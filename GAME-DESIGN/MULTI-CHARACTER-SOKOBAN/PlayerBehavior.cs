using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerBehavior : MonoBehaviour
{
    private List<Collider>[] positionalArray = { new(), new(), new(), new() };
    //private int currDir = 2;
    public bool echo = false;
    public bool invisible = false;

    [SerializeField]
    private GameObject[] bullets = new GameObject[4];
    // Start is called before the first frame update
    void Start()
    {
        //currDir = ((GameManager.Instance.currLevel == 1) ? 2 : 0);
    }

    // Update is called once per frame
    void Update()
    {
        if (!GameManager.Instance.inCutscene) {
            if (invisible) {
                gameObject.transform.localScale = Vector3.zero;
            } else {
                gameObject.transform.localScale = Vector3.one;
            }
            if (echo) {
                if (GameManager.Instance.newMove && GameManager.Instance.lastInput.Count > 1) {
                    GameManager.Instance.newMove = false;
                    if (GameManager.Instance.lastInput[^2] == KeyCode.W) {
                        //currDir = 0;
                        Move(3, -1);
                    } else if (GameManager.Instance.lastInput[^2] == KeyCode.S) {
                        //currDir = 2;
                        Move(3, 1);
                    } else if (GameManager.Instance.lastInput[^2] == KeyCode.A) {
                        //currDir = 3;
                        Move(1, 1);
                    } else if (GameManager.Instance.lastInput[^2] == KeyCode.D) {
                        //currDir = 1;
                        Move(1, -1);
                    } else if (GameManager.Instance.lastInput[^2] == KeyCode.Space) {
                        Shoot();
                    }
                }
            } else {
                if (Input.GetKeyUp(KeyCode.UpArrow) || Input.GetKeyUp(KeyCode.W)) {
                    GameManager.Instance.newMove = true;
                    GameManager.Instance.turnCount++;
                    GameManager.Instance.lastInput.Add(KeyCode.W);
                    Move(3, 1);
                } else if (Input.GetKeyUp(KeyCode.DownArrow) || Input.GetKeyUp(KeyCode.S)) {
                    GameManager.Instance.newMove = true;
                    GameManager.Instance.turnCount++;
                    GameManager.Instance.lastInput.Add(KeyCode.S);
                    Move(3, -1);
                } else if (Input.GetKeyUp(KeyCode.LeftArrow) || Input.GetKeyUp(KeyCode.A)) {
                    GameManager.Instance.newMove = true;
                    GameManager.Instance.turnCount++;
                    GameManager.Instance.lastInput.Add(KeyCode.A);
                    Move(1, -1);
                } else if (Input.GetKeyUp(KeyCode.RightArrow) || Input.GetKeyUp(KeyCode.D)) {
                    GameManager.Instance.newMove = true;
                    GameManager.Instance.turnCount++;
                    GameManager.Instance.lastInput.Add(KeyCode.D);
                    Move(1, 1);
                } else if (Input.GetKeyUp(KeyCode.Space)) {
                    GameManager.Instance.newMove = true;
                    GameManager.Instance.turnCount++;
                    GameManager.Instance.lastInput.Add(KeyCode.Space);
                }
            }
        }
    }

    public void UpdatePositionalArray(string tag, Collider tile) {
        switch (tag) {
            case "North":
                AddToList(0, tile);
                break;
            case "East":
                AddToList(1, tile);
                break;
            case "South":
                AddToList(2, tile);
                break;
            case "West":
                AddToList(3, tile);
                break;
        }
    }
    public void RemoveFromPositionalArray(string tag, Collider tile) {
        switch (tag) {
            case "North":
                positionalArray[0].Remove(tile);
                break;
            case "East":
                positionalArray[1].Remove(tile);
                break;
            case "South":
                positionalArray[2].Remove(tile);
                break;
            case "West":
                positionalArray[3].Remove(tile);
                break;
        }
    }

    public List<Collider> GetPositionalArray(string tag) {
        return tag switch {
            "North" => positionalArray[0],
            "East" => positionalArray[1],
            "South" => positionalArray[2],
            "West" => positionalArray[3],
            _ => null,
        };
    }

    private void Move(int axis, int dir) {
        switch (axis) {
            case 1:
                if(!ValidMovement((dir == 1) ? 1 : 3)) {
                    return;
                }
                break;
            case 3:
                if (!ValidMovement((dir == 1) ? 0 : 2)) {
                    return;
                }
                break;
        }
        transform.Translate(new Vector3(dir * 2 * ((axis == 1) ? 1 : 0), dir * 2 * ((axis == 2) ? 1 : 0), dir * 2 * ((axis == 3) ? 1 : 0)));
    }

    private bool ValidMovement(int dir) {
        bool valid = true;
        //Returns false if there is an obstacle in the way of the direction moved
        //North = 0, East = 1, South = 2, West = 3
        for (int i = 0; i < positionalArray[dir].Count; i++) {
            if (positionalArray[dir] == null) {
            } else if (positionalArray[dir][i].CompareTag("Wall") || positionalArray[dir][i].CompareTag("Drone")) {
                valid = false;
            } else if (positionalArray[dir][i].CompareTag("Crate")) {
                valid = valid && positionalArray[dir][i].GetComponent<CrateBehavior>().ValidMovement(dir);
                positionalArray[dir][i].GetComponent<CrateBehavior>().FinishedMove(dir);
            }
        }


        //Returns whether or not the player is 'dead' and/or if the player has already exited
        return (valid && !GameManager.Instance.loseCondition && !invisible);
    }

    private void Shoot() {
        if (GameManager.Instance.currLevel > 1)
        {
            for (int i = 0; i < bullets.Length; i++)
            {
                bullets[i].SetActive(true);
                StartCoroutine(BulletTimer(i));
            }
        }

    }

    IEnumerator BulletTimer(int dir) {
        yield return new WaitForSeconds(.28f);
        bullets[dir].SetActive(false);
        bullets[dir].GetComponent<BulletCollider>().Clear();
    }

    public void GameOver() {
        GameManager.Instance.loseCondition = false;
        GameManager.Instance.winCondition = 0;
        GameManager.Instance.buttCondition = false;
        GameManager.Instance.lastInput.Clear();
        SceneManager.LoadScene(SceneManager.GetActiveScene().name);
    }

    public void AddToList(int dir, Collider tile) {
        bool validCollider = true;
        for (int i = 0; i < positionalArray[dir].Count; i++) {
            if (tile == positionalArray[dir][i]) {
                validCollider = false;
            }
        }
        if (validCollider) {
            positionalArray[dir].Add(tile);
        }
    }
}
