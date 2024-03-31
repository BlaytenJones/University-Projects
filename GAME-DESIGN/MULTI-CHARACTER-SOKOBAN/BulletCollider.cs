using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BulletCollider : MonoBehaviour {
    private List<Collider> objectsInView = new();

    private void OnTriggerEnter(Collider other) {
        if (other.CompareTag("Wall") || other.CompareTag("Crate") || other.CompareTag("Drone")) {
            objectsInView.Add(other);
        }
    }


    private void OnTriggerExit(Collider other) {
        if (other.CompareTag("Wall") || other.CompareTag("Crate") || other.CompareTag("Drone")) {
            objectsInView.Remove(other);
        }
    }

    public void Clear() {
        objectsInView.Clear();
    }

    [System.Obsolete]
    private void Update() {
        if (objectsInView.Count > 0) {
            int closest = 0;
            for (int i = 1; i < objectsInView.Count; i++) {
                if (Vector3.Distance(objectsInView[i].transform.position, transform.parent.position) < Vector3.Distance(objectsInView[closest].transform.position, transform.parent.position)) {
                    closest = i;
                }
            }
            if (objectsInView[closest].CompareTag("Drone")) {
                objectsInView[closest].transform.FindChild("Drone").gameObject.SetActive(false);
                objectsInView[closest].gameObject.tag = "Walkable";
                objectsInView.Remove(objectsInView[closest]);
            }
        }
    }
}